# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

module load gcc/4.8.2
module load python/2.7.10
#module load matlab/R2015a
#module avail

export SVN_EDITOR=vim
LIBFFI_HOME=${HOME}/Documents/libffi.inst
LLVM_3_4_2_HOME=$HOME/Install/llvm-3.4.2.install
LLVM_3_5_0_HOME=$HOME/Install/llvm-3.5.0.install
LLVM_HOME=$LLVM_3_5_0_HOME
LLVM_3_1_HOME=$HOME/Documents/llvm/llvm-3.1.inst/
LLVM_2_9_HOME=$HOME/llvm/llvm-2.9/
ZESTI_HOME=$HOME/Documents/klee-zesti/zesti.obj/Release+Asserts/bin/
KLEE_HOME=$HOME/Work/klee.3.4/Release+Asserts/
CMAKE_HOME=$HOME/Install/cmake-3.3.2
GDB_HOME=$HOME/Install/gdb-7.10
DDD_HOME=$HOME/Install/ddd-3.3.12-install
NASM_HOME=$HOME/Install/nasm-2.12.01.install/

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

export PATH=$NASM_HOME/bin:$DDD_HOME/bin:$LLVM_HOME/bin:$CMAKE_HOME/bin/:$GDB_HOME/gdb/:$PATH
export LD_LIBRARY_PATH=$LLVM_HOME/lib/:$LIBFFI_HOME/lib64:$LD_LIBRARY_PATH

## Setting env vars for ICC
source ${HOME}/Install/intel/bin/compilervars.sh intel64

if [ -f ~/.aliases ]; then
  . ~/.aliases
fi

# Some commands
#tail -n 20  ~/commands.txt
git config --global user.name "Sandeep Dasgupta"
git config --global user.email "sdasgup3@illinois.edu"

bind -f  ~/.inputrc
