#!/usr/bin/perl

use strict;
use warnings;
use Getopt::Long;


my $message = "";
my @filenames = "";

GetOptions (
            "m=s"     => \$message, 
            "files=s"    => \@filenames, 
            ) or die("Error in command line arguments\n");


execute("git pull");
execute("git add @filenames");
execute("git ci -m \"$message\" @filenames ");
execute("git push ");


sub execute {
  my $args = shift @_;
  print "$args \n";
  system("$args");
}


 
