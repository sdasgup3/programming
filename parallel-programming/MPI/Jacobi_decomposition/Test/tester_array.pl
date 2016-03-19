#!/usr/bin/perl

use warnings;
use strict;


my @sizes = (144, 288, 432);
my @cores = (1,12,24,36);


#foreach my $size (@sizes) {
#        print "Running Jacobi on size $size\n";
#        system("../jacobi $size 1000 1 1 > gold_$size");
#}

foreach my $size (@sizes) {
    foreach my $core (@cores) {
        system("echo \"mpirun -n  $core ../pjacobi $size 1000 1 1 > out_1\"");
        system("mpirun -n  $core ../pjacobi $size 1000 1 1 > out_1");

        system("echo \"mpirun -n  $core ../coljacobi $size 1000 1 1 > out_2\"");
        system("mpirun -n  $core ../coljacobi $size 1000 1 1 > out_2");

        print "Diffing PJacobi/COlJacobi on size $size and core $core\t:";
        my $res = `diff3 out_1 out_2  gold_$size`;
        #my $res = `diff out_2  gold_$size`;
        if("" eq $res) {
            print "Pass \n";
            system("rm -rf out_1 out_2");
            #system("rm -rf out_2 ");
        } else {
            print "Diff Exists\n";
            exit(0);
	}
    }
}
