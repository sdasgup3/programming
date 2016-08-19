### Installations
  - cmake
  ```
    sudo apt-get install g++
    cd cmake
    ./configure
    make
  ```

  - nasm
  ```
  http://www.nasm.us/
   ./configure  --prefix=/home/ubuntu/Install/nasm.install
   make 
   make install
  ```

  - llvm
  
  ```
  cd llvm
  mkdir build; cd  build
  cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DLLVM_ENABLE_ASSERTIONS=ON -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX=/home/ubuntu/Install/llvm-3.5.0.install -DLLVM_TARGETS_TO_BUILD="all"  ..
  make ENABLE_OPTIMIZED=1 DISABLE_ASSERTIONS=0 -j 8
  make install
  ```
  - mcsema
  ```
  git clone git@github.com:trailofbits/mcsema.git
  cd mcsema
  sudo apt-get install git gcc-multilib g++-multilib build-essential cmake libc6-i386 nasm
  mkdir build
  cd build
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
  make
  ```
