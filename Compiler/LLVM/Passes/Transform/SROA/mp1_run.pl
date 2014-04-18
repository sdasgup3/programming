#!/usr/bin/perl

use Getopt::Long;
use strict;
use warnings;

my $test = "";
my $regen = "";
my $clean = "";
my $diff = "";
my @pass ;

GetOptions (
            "test=s"    => \$test, 
            "regen"    => \$regen, 
            "clean"    => \$clean, 
            "diff"    => \$diff, 
#            "pass=s"    => \@pass, 
            ) 
 or die("Error in command line arguments\n");

my $file = "fileList";

open (my $FP, "<" , $file);
my @fileList = <$FP>;

foreach my $file (@fileList) {
  chomp $file;
  system("~/Scripts/mp1_build.pl -test $file -stats -diff");
}
