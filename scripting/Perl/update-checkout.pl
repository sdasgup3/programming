#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
use File::chdir;
use Cwd;

my $home="";
my $checkout="";
my $push="";
my $pull="";
my $rebase="";
my $llvm="";
my $clang="";
my $swift="";
my $show="";
my $current=getcwd();
my $help="";
my %tool_branch;

my @auxdirs1= ( 
  "compiler-rt",
  "swiftpm",
  "llbuild",
  "cmark",	
  "lldb",
);


$tool_branch{'swift'} = 'clean';
$tool_branch{'llvm'} = 'clean';
$tool_branch{'clang'} = 'clean';

my @maindirs= ( 
  "self-webifying",	
  "coding-puzzles",
  "binary-decompilation",
  "cheatsheet",
  "compilers",
  "parallel-programming",
  "partial-redundancy-elimination",
  "gri",
);

my @all= ();
push(@all, @maindirs);




GetOptions (
            "help"          => \$help, 
            "checkout"      => \$checkout, 
            "push"          => \$push, 
            "pull"          => \$pull, 
            "rebase"        => \$rebase, 
            "llvm"          => \$llvm, 
            "clang"         => \$clang, 
            "swift"         => \$swift, 
            "show"          => \$show, 
            ) or die("Error in command line arguments\n");

if($help) {
  helper();
  exit(1);
}

sub helper {
  print ("merge_swift.pl -checkout/-show/-merge/-pull/-push  \n");
}


if("" ne $checkout) {
  sub_checkout();
}

if("" ne $show) {
  sub_show();
}

if("" ne $push) {
  sub_push();
}

if("" ne $pull) {
  sub_pull();
}

if("" ne $rebase) {
  sub_rebase();
}

sub sub_show {
  foreach my $tool  (@all) {
    $CWD = "$tool";
    print ("\n\nTool:  $tool \n");
    print ("\tRemotes \n");
    execute("git remote -v");
    print ("\tBranches \n");
    execute("git  branch -a");
    $CWD = "$current";
  }
}

sub sub_pull {
  foreach my $tool  (@all) {
    $CWD = "$tool";
    print ("\n\nPulling Tool from remote stash:  $tool \n");
    execute("git pull origin master");
    $CWD = "$current";
  }
}

sub sub_push {
  foreach my $tool  (@all) {
    $CWD = "$tool";
    print ("\n\nPushing Tool to remote stash:  $tool \n");
    execute("git push stash $tool_branch{$tool}");
    $CWD = "$current";
  }
}

sub sub_checkout {
  foreach my $dir  (@all) {
    print ("\n\nCheckout $dir \n");
    execute("git clone git\@github.com:sdasgup3/${dir}.git");
    #$CWD = "$dir";
    #execute("git remote rename origin stash");
    #execute("git remote add origin git\@github.com:apple/swift-$dir.git");
    #execute("git remote set-url --push origin no_push");
    #$CWD = "$current";
  }
}

sub sub_rebase {
  my $dir="";

  if("" ne $llvm) {
    $dir = "llvm";
    print ("\n\nMerging $dir \n");
    $CWD = "$dir";

    execute("git stash ");
    execute("git fetch origin stable");
    execute("git rebase FETCH_HEAD");
    execute("git stash apply ");

    $CWD = "$current";
    return;
  }

  if("" ne $clang) {
    $dir = "clang";
    print ("\n\nMerging $dir \n");
    $CWD = "$dir";

    execute("git stash ");
    execute("git fetch origin stable");
    execute("git rebase FETCH_HEAD");
    execute("git stash apply ");

    $CWD = "$current";
    return;
  }


  if("" ne $swift) {
    $dir = "swift";
    $CWD = "$dir";

    execute("git stash ");
    execute("git fetch origin master");
    execute("git rebase FETCH_HEAD");
    execute("git stash apply ");

    $CWD = "$current";
    return;
  }


}



sub execute {
  my $args = shift @_;
  print "EXECUTING: $args \n";
  system("$args");
}

