# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

module load gcc/4.8.2
module load python/2.7.3-x86_64

export SVN_EDITOR=vim
export LIBFFI_HOME=/home/sdasgup3/Documents/libffi.inst
export LLVM_HOME=$HOME/Documents/llvm/llvm.inst
export LLVM_2_9_HOME=$HOME/llvm/llvm-2.9/
export ZESTI_HOME=$HOME/Documents/klee-zesti/zesti.obj/Release+Asserts/bin/
export KLEE_HOME=$HOME/Work/klee.3.4/Release+Asserts/bin/

export PATH=$LLVM_HOME/bin:$ZESTI_HOME:$KLEE_HOME:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBFFI_HOME/lib64

if [ -f ~/.aliases ]; then
  . ~/.aliases
fi

# Some commands
tail -n 18  ~/commands.txt
git config --global user.name "Sandeep Dasgupta"
git config --global user.email "sdasgup3@illinois.edu"
