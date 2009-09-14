#!/usr/local/perl

use strict;
use warnings;

open my $fh, "<", $ARGV[0]; 

my @x; 
my @y; 
my @z;

while (<$fh>) {
    chomp; 
    my ($a, $b, $c) = split /\t/, $_; 
    if (($a < 1000) && (1000 < $c && $c < 2000)) {
	push @x, int $a;
	push @y, int $b;
	push @z, int $c;
    }
}

# @x = sort {$a <=> $b} @x;
# @y = sort {$a <=> $b} @y;
# @z = sort {$a <=> $b} @z;

for (my $i=0; $i<@x; $i++) {
    print $x[$i], "\t", $y[$i], "\t", $z[$i], "\n";
}
