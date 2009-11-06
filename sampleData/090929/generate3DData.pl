#!/usr/bin/perl

use strict;
use warnings;

open my $fh, "<", $ARGV[0];

while (<$fh>) {
    my ($x, $y, $z, $u, $v) = split /\t/, $_;
    print $x, "\t", $y, "\t", $z, "\n";
}
