#!/usr/bin/perl

use strict;
use warnings;
use Getopt::Long;


my $message = "";
my @filenames = "";
my $help = "";

GetOptions (
            "m=s"     => \$message, 
            "files=s"    => \@filenames, 
            "help"    => \$help, 
            ) or die("Error in command line arguments\n");

if($help) {
  print "Usage: mygit -m -files  \n";
  exit(1);
}

execute("git pull");
execute("git add @filenames");
execute("git ci -m \"$message\" @filenames ");
execute("git push ");


sub execute {
  my $args = shift @_;
  print "$args \n";
  system("$args");
}


 
