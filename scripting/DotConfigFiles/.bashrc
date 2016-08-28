# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

#module load gcc/4.8.2
module load gcc/5.3
#module load python/2.7.10
#module load matlab/R2015a
#module avail

export SVN_EDITOR=vim
CMAKE_HOME=$HOME/Install/cmake
NINJA=${HOME}/Github/ninja
GDB_HOME=$HOME/Install/gdb-7.10
DDD_HOME=$HOME/Install/ddd-3.3.12-install
PERL=${HOME}/Install/perl.install

echo "=================================================="
echo $PATH
echo "=================================================="

export PATH=${PERL}/bin:${NINJA}:$DDD_HOME/bin:$CMAKE_HOME/bin/:$GDB_HOME/gdb/:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

## Setting env vars for ICC
#source ${HOME}/Install/intel/bin/compilervars.sh intel64

if [ -f ~/.aliases ]; then
  . ~/.aliases
fi

# Git commands
git config --global user.name "Sandeep Dasgupta"
git config --global user.email "sdasgup3@illinois.edu"

bind -f  ~/.inputrc
export PATH="$(echo $PATH | perl -e 'print join(":", grep { not $seen{$_}++ } split(/:/, scalar <>))')"
