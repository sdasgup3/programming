#!/usr/bin/perl


use warnings;
use strict;

my $fp;
my $basedir = "/home/sandeep/CS-PHD/Sem1/TA-427/fa13-cs427/";
my $inputFile = $ARGV[0];
open ($fp, "<", $inputFile) or die "cannot open < $inputFile: $!";

my @lines = <$fp>;
close ($fp);


for my $line (@lines) {
  chomp $line;

  my @tokens = split(/[ ][ ]*/, $line);

  my @copy_tokens = @tokens;

  print "\nCreating Evaluation Forms for Team $tokens[0]\n";

  my $num_members = @tokens ; 

  for (my $i = 1 ; $i < $num_members; $i ++) {
    print "\tCreating Evaluation Forms for Team member $tokens[$i]\n";

    my $evalfile = "$basedir/$tokens[$i]/evaluation.txt";
    open($fp, ">", "$evalfile") or die "cannot open < $evalfile: $!";;

    &writeform($fp, $tokens[$i], @tokens);
    close($fp);
  }
}

sub writeform
{
  my ($fp, $self, @copy_tokens) = @_;

  my $num_members = @copy_tokens;

  print $fp "Peer Evaluation\n";
  print $fp "===============\n";
  print $fp "Q. Suppose you have been given \$1050 and you are asked to distribute\n"; 
  print $fp "the money among the OTHER team members based on their contribution to \n"; 
  print $fp "the final team project.\n\n"; 
  print $fp "    netids          Amount given\n";
  print $fp "    -----------------------------\n";

  for (my $j = 1 ; $j < $num_members; $j ++) {
    next if ($self eq $copy_tokens[$j]);

    print $fp "   $copy_tokens[$j]\n";  
  }

  print $fp "                  --------------\n";
  print $fp "                  Total = \$1050\n";
  print $fp "                  --------------\n\n";

  print $fp "Q. For each member of the teams, answer the followings:\n\n";
  for(my $j = 1 ; $j < $num_members; $j ++) {
    next if ($self eq $copy_tokens[$j]);

    print $fp "Evaluation of $copy_tokens[$j]\n";
    print $fp "-----------------------------\n";
    print $fp "\* What did you do together and how did you work together?\n\n\n"; 
    print $fp "\* Did $copy_tokens[$j] cooperate(Y/N)?\n"; 
    print $fp "  \* Why do you think so? Illustrate with an example.\n\n\n"; 
    print $fp "\* Did $copy_tokens[$j] fulfill his/her commitments(Y/N)?\n"; 
    print $fp "  \* Why do you think so? Illustrate with an example.\n\n\n"; 
    print $fp "\* Do you feel that $copy_tokens[$j] contributed positively to the project (Y/N)?\n";
    print $fp "  \* Why do you think so? Illustrate with an example.\n\n\n"; 
  }

  print $fp "\n\n";
  print $fp "Self Evaluation\n";
  print $fp "===============\n";
  print $fp "\* Did you fulfill your commitments (Y/N)?\n";
  print $fp "  \* Why do you think so? Illustrate with an example.\n\n\n"; 
  print $fp "\* Do you feel that you contributed positively to the project (Y/N)?\n";
  print $fp "  \* Why do you think so? Illustrate with an example.\n\n\n"; 

}




