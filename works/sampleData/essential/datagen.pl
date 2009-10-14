#! /usr/bin/perl

# check exit and die
# how to print help message in perl (using perldoc?)

use strict;
use warnings;
use Getopt::Long;

open my $fh, "<", $ARGV[0] or die "type \"$0 -h\" for displaying help message\n";

my ($xmin, $xrange, $ymin, $yrange, $zmin, $zrange, $delimiter, $help);

GetOptions("xmin=i" => \$xmin,
	   "xrange=i" => \$xrange,
	   "ymin=i" => \$ymin,
	   "yrange=i" => \$yrange,
	   "zmin=i" => \$zmin,
	   "zrange=i" => \$zrange,
	   "delimiter|del=s" => \$delimiter,
	   "h|help" => \$help);

printHelp() if $help;

$xmin = -100000 if !defined($xmin);
$xrange = 200000 if !defined($xrange);
$ymin = -100000 if !defined($ymin);
$yrange = 200000 if !defined($yrange);
$zmin = -100000 if !defined($zmin);
$zrange = 200000 if !defined($zrange);
$delimiter = "\t" if !defined($delimiter);

for my $line (<$fh>) {
    my ($x, $y, $z, $u, $v) = split /\t/, $line;
    next if (int($x) == -1 || int($y) == -1 || int($z) == -1);
    next if (int($x) == 0 && int($y) == 0 && int($z) == 0);
    print int($x), $delimiter, int($y), $delimiter, int($z), "\n"
	if $x >= $xmin && $x <= $xmin + $xrange
	&& $y >= $ymin && $y <= $ymin + $yrange
	&& $z >= $zmin && $z <= $zmin + $zrange;
}

sub printHelp {
    while (<DATA>) {
	print $_;
    }
    exit 0;
}

__DATA__
Usage:
    datagen.pl <coordinate file name> [--xmain --xrange --ymin --yrange --zmin --zrange --delimiter]

SYNOPSYS:
    datagen.pl foo.txt --xmin=-1000 xrange=2000
