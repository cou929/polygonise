#/usr/bin/perl

use strict;
use warnings;

my $MAX = 4;

open my $fh, "<", "../../poly.conf";

my $round = 1;

while (<$fh>) {
    if (/round/) {
	(my $name, $round) = split /\t/, $_;
    }
}

if ($round < 10) {
    $round = "0" . $round;
}

for (my $i = 1; $i <= $MAX; $i++) {
    my $num = $i;
    if ($i < 10) {
	$num = "0" . $i;
    }
    system "./imageValFeature " . $num . "_depth.png " . $num . "_coord.txt > " . $num . "_imgCoord.txt";
    system "../../gen " . $num . "_imgCoord.txt ../../poly.conf > ../../stl/img_human_red" . $round . "_" . $num . ".stl";
}
