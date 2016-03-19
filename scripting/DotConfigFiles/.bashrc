# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

module load gcc/4.8.2
module load python/2.7.10
#module load matlab/R2015a
#module avail

export SVN_EDITOR=vim
export LIBFFI_HOME=/home/sdasgup3/Documents/libffi.inst
export LLVM_3_4_2_HOME=$HOME/Install/llvm-3.4.2.install
export LLVM_HOME=$LLVM_3_4_2_HOME
export LLVM_3_1_HOME=$HOME/Documents/llvm/llvm-3.1.inst/
export LLVM_2_9_HOME=$HOME/llvm/llvm-2.9/
export ZESTI_HOME=$HOME/Documents/klee-zesti/zesti.obj/Release+Asserts/bin/
export KLEE_HOME=$HOME/Work/klee.3.4/Release+Asserts/
export CMAKE_HOME=$HOME/Install/cmake-3.3.2
export GDB_HOME=$HOME/Install/gdb-7.10
export DDD_HOME=$HOME/Install/ddd-3.3.12-install

#export SELF_HOME=$HOME/Install/self.install
#export SMALLTALK_HOME=$HOME/Install/pharo4.0/
#export NG_HOME=$HOME/Documents/klee-zesti/neongoby.inst/
#export VALTOOL_HOME=/home/sdasgup3/Projects/mcsema/mc-sema/validator/x86_64/valTest/../valTool/
#export FLEX_HOME=$HOME/Install/flex.install
#export BISON_HOME=$HOME/Install/bison.install
#export M4_HOME=$HOME/Install/m4.install
#export M4_HOME=
#export GRAPHAL_HOME=$HOME/Install/graphal-1.0.1/build/ 
#export MAIL_HOME=$HOME/Install/
#export QT_HOME=$HOME/Qt/5.5/gcc_64/ 
#export GRI_HOME=$HOME/GRI/source
#export IMAKE_HOME=$HOME/Install/imake.obj

export PATH=$DDD_HOME/bin:$IMAKE_HOME/bin/:$QT_HOME/bin:$MAIL_HOME:$BISON_HOME/bin/:$M4_HOME/bin/:$FLEX_HOME/bin/:$NG_HOME/bin/:$LLVM_HOME/bin:$ZESTI_HOME:$KLEE_HOME/bin/:$CMAKE_HOME/bin/:$SELF_HOME/vm:$SMALLTALK_HOME/:$GDB_HOME/gdb/:$PATH
export LD_LIBRARY_PATH=/home/sdasgup3/Install/llvm-3.4.2.install/lib/:/home/sdasgup3/GitHub/LLVMSlicer.obj/src/:$LD_LIBRARY_PATH:$LIBFFI_HOME/lib64:$GRAPHAL_HOME/libgraphal/:$FLEX_HOME/lib/:$GRI_HOME/build/lib/:$VALTOOL_HOME

if [ -f ~/.aliases ]; then
  . ~/.aliases
fi

# Some commands
#tail -n 20  ~/commands.txt
git config --global user.name "Sandeep Dasgupta"
git config --global user.email "sdasgup3@illinois.edu"
