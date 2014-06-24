#!/usr/bin/perl

use Getopt::Long;
use strict;
use warnings;

my $test = "";
my $maxt = "";
my $result = "";
my $reuse = "";
my $genexec = "";
my $run = "";
my $watch = "";
my $modifiedll = "";
my $withoutcheker = "";
my @progargs = "";

GetOptions ("wc"        => \$withoutcheker, 
            "test=s"    => \$test, 
            "result"    => \$result, 
            "reuse"    => \$reuse, 
            "genexec"    => \$genexec, 
            "run"     => \$run, 
            "watch"     => \$watch, 
            "args=s"     => \@progargs, 
            "maxt=s"     => \$maxt, 
            "mdll"      => \$modifiedll) 
 or die("Error in command line arguments\n");

my $make        = "make -f ~/Scripting/Makefile"; 

###  LLVM Args
my $llvm_bin_2_9    = "/home/sdasgup3/llvm/llvm-2.9/Release+Asserts/bin"; 
my $llvm_bin_3_4    = "/home/sdasgup3/llvm/llvm-llvmpa/llvm-build/Release+Asserts/bin/";
my    $llvmpalib    = "/home/sdasgup3/llvmpa/llvmpa-build/Release+Asserts/lib/";


my $clang2_9       = "$llvm_bin_2_9/clang";
my $llvmdis2_9     = "$llvm_bin_2_9/llvm-dis";
my $llvmas2_9      = "$llvm_bin_2_9/llvm-as";
my $llvmld2_9      = "$llvm_bin_2_9/llvm-ld";

###  Klee Args
my $runkleetest = "~/Scripting/runseq";
my $watchV      = "~/Scripting/watchV";
my $kleeargs    = "";
my $maxtime     = "";

if("" eq $maxt) {
  print "Setting max time to default 172800\n";
  $maxtime = "172800";
} else {
  $maxtime = $maxt;
}

# $kleeargs  = "-write-test-info";
# $kleeargs  = "--libc=uclibc  --allow-external-sym-calls";
# $kleeargs  = "--emit-all-errors";

#$kleeargs = "--libc=uclibc --allow-external-sym-calls  --max-time=$maxtime";
$kleeargs = 
      " --simplify-sym-indices --write-cvcs --write-cov --output-module"
    . " --max-memory=1000 --disable-inlining --optimize --use-forked-solver" 
    . " --use-cex-cache --libc=uclibc --posix-runtime"
    . " --allow-external-sym-calls --only-output-states-covering-new" 
#    . " --environ=/home/sdasgup3/Scripting/test.env --run-in=/tmp/sandbox" 
    . " --max-sym-array-size=4096 --max-instruction-time=120 --max-time=$maxtime" 
    . " --watchdog --max-memory-inhibit=false --max-static-fork-pct=1" 
    . " --max-static-solve-pct=1 --max-static-cpfork-pct=1 --switch-type=internal" 
    . " --randomize-fork --search=random-path --search=nurs:covnew" 
    . " --use-batching-search --batch-instructions=10000"; 
#./paste.bc --sym-args 0 1 10 --sym-args 0 2 2 --sym-files 1 8 --sym-stdout "; 


if("" ne $result) {
  system("cat $runkleetest");
  system("tcsh $runkleetest");
  system("cat $watchV");
  system("tcsh $watchV");
  exit(0);
}
if("" ne $run) {
  system("cat $runkleetest");
  system("tcsh $runkleetest");
  exit(0);
}
if("" ne $watch) {
  system("cat $watchV");
  system("tcsh $watchV");
  exit(0);
}

if("" ne $reuse) {
  if(-e "./a.out.bc") {
    execute("klee $kleeargs ./a.out.bc @progargs");
  }
  exit(0);
}

if($withoutcheker ne "") {
  execute("$make clean");
  execute("rm -rf  klee-*");
  execute("$clang2_9 -O0 -emit-llvm -I /home/sdasgup3/klee/klee/include/klee -I ./ -c $test.c -o a.out.bc");
  execute("$llvmdis2_9 a.out.bc -o a.out.ll");

  if("" eq $genexec) {
    execute("klee $kleeargs ./a.out.bc @progargs");
  }
  execute("echo");
  execute("echo");
  exit(0);
}


if(defined($test)) {
  if($modifiedll eq "") {
    execute("$make clean");
    execute("$make $test LLVM_BIN=$llvm_bin_3_4 LLVMPALIB=$llvmpalib");
    execute("echo");
    execute("$make $test-kleecheck LLVM_BIN=$llvm_bin_3_4 LLVMPALIB=$llvmpalib");
    exit(0);
    execute("cat $test-kleecheck.ll | sed 's/target datalayout.*//' | sed 's/\!llvm.ident =.*//' | sed 's/\!0 = metadata.*//' >  temp ");
    execute("mv temp $test-kleecheck.ll") ;
    execute("echo");
    execute("echo");
  }
  exit(0);
  execute("$clang2_9 -emit-llvm -c ~/Scripting/jf_checker_map.cpp -I ~/Scripting -o jf_checker_map.bc");
  execute("$llvmas2_9 < $test-kleecheck.ll  > a.bc");
  execute("$llvmld2_9 a.bc  jf_checker_map.bc");
  if("" eq $genexec) {
    execute("klee $kleeargs ./a.out.bc");
  }
  execute("echo");
  execute("echo");
  exit(0);
}



sub execute {
  my $args = shift @_;
  print "$args \n";
  system("$args");
}
