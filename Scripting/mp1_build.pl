#!/usr/bin/perl

use Getopt::Long;
use strict;
use warnings;

my $test = "";
my $regen = "";
my $clean = "";
my $diff = "";
my $stats = "";
my @pass ;

GetOptions (
            "test=s"    => \$test, 
            "regen"    => \$regen, 
            "clean"    => \$clean, 
            "diff"    => \$diff, 
            "stats"    => \$stats, 
#            "pass=s"    => \@pass, 
            ) 
 or die("Error in command line arguments\n");

my $make        = "make -f ~/Scripts/Makefile"; 

###  LLVM Args
#my $llvm_bin  = "/home/sdasgup3/work/llvm-build/Debug+Asserts/bin/";
my $llvm_bin  = "/home/sdasgup3/LLVM/Latest/build/Release+Asserts/bin/";


my $clang       = "$llvm_bin/clang";
my $llvmdis     = "$llvm_bin/llvm-dis";
my $llvmas      = "$llvm_bin/llvm-as";
my $llvmld      = "$llvm_bin/llvm-ld";
my $opt         = "$llvm_bin/opt";

if($clean ne "") {
  execute("rm -rf *.ll *.bc *.O");
  exit(0);
}

my $opts = "";
if("" ne $stats) {
#$opts = "-stats -time-passes";
  $opts = "-stats ";
}

if($regen ne "") {
  execute("rm -rf $test.bc $test.ll $test-mem2reg.bc $test-mem2reg.ll $test-scalarrepl.bc $test-scalarrepl.ll $test-scalarrepl-sdasgup3.bc $test-scalarrepl-sdasgup3.ll");
}

execute("$clang -O0 -emit-llvm -c $test.c -o $test.bc");
execute("$llvmdis $test.bc -o $test.ll");

#execute("$opt  -mem2reg $opts  < $test.bc > $test-mem2reg.bc");
#execute("$llvmdis $test-mem2reg.bc -o $test-mem2reg.ll");
#execute("rm -rf $test-mem2reg.bc ");

execute("$opt  -scalarrepl  $opts  $test.bc -o  $test-scalarrepl.bc |& tee $test-log");
execute("$llvmdis $test-scalarrepl.bc -o $test-scalarrepl.ll");
execute("rm -rf $test-scalarrepl.bc ");
execute("cut -d ' ' -f1 $test-log > $test-scalarrepl.O ");

execute("$opt -load /home/sdasgup3/LLVM/Latest/build/Debug+Asserts/lib/libLLVMSROA.so -scalarrepl-sdasgup3 $opts  $test.bc -o  $test-scalarrepl-sdasgup3.bc |& tee $test-log");
execute("$llvmdis $test-scalarrepl-sdasgup3.bc -o $test-scalarrepl-sdasgup3.ll");
execute("rm -rf $test-scalarrepl-sdasgup3.bc ");
execute("cut -d ' ' -f1 $test-log > $test-scalarrepl-sdasgup3.O ");

if("" ne $diff) {
  print "Diffing($test) \n";

  my $result = system("diff $test-scalarrepl.ll $test-scalarrepl-sdasgup3.ll > $test-log");
  $result = `grep -v 'ModuleID' $test-log | grep -v "1c1" | grep -v '-'`;
  print "$result";
  if("" eq $result) {
    log_pass();
  } else {
    if(-e "$test-scalarrepl-sdasgup3.ll.G") {
      $result = `diff $test-scalarrepl-sdasgup3.ll.G $test-scalarrepl-sdasgup3.ll > /dev/null`;
      if("" eq $result) {
        log_pass();
      } else {
        print "G Log Fail ($test)\n ";
      }
    } else {
      print "Log Fail ($test)\n ";
    }
  }

  $result = `diff $test-scalarrepl-sdasgup3.O $test-scalarrepl.O`;
  if("" eq $result) {
    print "\tOut Pass\n ";
    execute("rm -rf $test-scalarrepl-sdasgup3.O $test-scalarrepl.O ");
  } else {
    if(-e "$test-scalarrepl-sdasgup3.O.G") {
      $result = `diff $test-scalarrepl-sdasgup3.O.G  $test-scalarrepl-sdasgup3.O > /dev/null`;
      if("" eq $result) {
        print "\tOut Pass\n ";
        execute("rm -rf $test-scalarrepl-sdasgup3.O $test-scalarrepl.O ");
      } else {
        print "\tG Out Fail ($test)\n";
      } 
    } else {
      print "\tOut Fail($test) \n";
    }

  }
  execute("rm -rf $test-log");
    
}


exit(0);

sub log_pass {
  print "\tLog Pass\n ";
  execute("rm -rf $test.bc $test.ll $test-mem2reg.bc $test-mem2reg.ll $test-scalarrepl.bc $test-scalarrepl.ll $test-scalarrepl-sdasgup3.bc $test-scalarrepl-sdasgup3.ll");
}


sub execute {
  my $args = shift @_;
#print "$args \n";
  system("$args");
}
