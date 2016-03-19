#!/bin/bash

# location where clang should be installed
#INSTALL_PREFIX=$HOME/Install/llvm-trunk-with-gcc-install
INSTALL_PREFIX=$HOME/Install/llvm-3.4.2.install
SRC_DIR=$HOME/Install/llvm-3.4.2.src

# location of gcc used to build clang
HOST_GCC=/software/gcc-4.8.2/

# number of cores
CPUS=16
# uncomment following to get verbose output from make
#VERBOSE=VERBOSE=1
# uncomment following if you need to sudo in order to do the install
#SUDO=sudo

if [ $1 = "gcc" ]; then
  C="${HOST_GCC}/bin/gcc" ; 
  CXX="${HOST_GCC}/bin/g++" ;
else
  C="/home/sdasgup3/Install/llvm-trunk-install/bin/clang" ; 
  CXX="/home/sdasgup3/Install/llvm-trunk-install/bin/clang++" ;
fi  

echo $C



#
# gets clang tree from svn into ./llvm
# params (e.g., -r) can be specified on command line
#
cmake                     -DCMAKE_C_COMPILER=$C \
                          -DCMAKE_CXX_COMPILER=$CXX \
                          -DGCC_INSTALL_PREFIX=${HOST_GCC} \
                          -DCMAKE_CXX_LINK_FLAGS="-L${HOST_GCC}/lib64 -Wl,-rpath,${HOST_GCC}/lib64" \
                          -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
                          -DLLVM_ENABLE_ASSERTIONS=ON \
                          -DCMAKE_BUILD_TYPE="Release" -DLLVM_TARGETS_TO_BUILD="all" \
                          $SRC_DIR
#make -j ${CPUS} ${VERBOSE}
#make install
