#!/usr/bin/perl
use strict;
use warnings;
use File::Basename;

my $file = $ARGV[0];


my ($filename, $dir, $suff) = fileparse($file,qr/.tex/);
system("pdflatex $filename");
system("rm -rf *.dvi  *.log *.ps *.aux *.out");
