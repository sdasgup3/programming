#!/usr/bin/perl

use warnings;
use strict;


#my @sizes = (1200, 12000, 120000,  1200000, 12000000, 120000000);
my @sizes = (120000, 1200000,  12000000);
my @cores = (2,6,12);


foreach my $size (@sizes) {
    foreach my $core (@cores) {
        system("mpirun -n  $core ../phistogramSort $size > out");

        print "Diffing psamplesort on size $size and core $core\t:";
        my $res = `diff out  gold_$size`;
        if("" eq $res) {
            print "Pass \n";
            system("rm -rf out");
        } else {
            print "Diff Exists\n";
            exit(0);
	}
    }
}
