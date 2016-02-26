## Command Usage ##
- C program to LLVM code  (.ll) using clang
`$ clang -S -emit-llvm myfile.c -o myfile.ll`

- C program to llvm bytecode
`$ clang -c -emit-llvm myfile.c -o myfile.bc`

- The bytecode files are not human readable. However you can transform from bytecode to text and 
vice versa using some LLVM tools:
`$ llvm-dis myfile.bc -o myfile.ll`
`$ llvm-as myfile.ll -o myfile.bc`

- For building a llvm based project outside the llvm build framework
  - Copy llvm-src/projects/sample
```
$ mkdir build
$ cd build
$ ../sample/configure --with-llvmsrc=/home/sdasgup3/Documents/llvm/llvm.src/ --with-llvmobj=/home/sdasgup3/Documents/llvm/llvm.obj/
$ make ENABLE_OPTIMIZED=1   
```
- Run an LLVM Pass With Clang
  - `clang -Xclang -load -Xclang mypass.so *.c`
  - An [example](https://github.com/sdasgup3/Programming/tree/master/Compiler/LLVM/LLVMProjects/lib/Analysis/opModifier)

## Links ##
- For more about various LLVM tools: http://llvm.org/docs/CommandGuide/index.html
- For moe information about creating a LLVM project :  http://llvm.org/releases/3.7.1/docs/Projects.html
- Frequently asked Questions: http://llvm.org/releases/3.1/docs/FAQ.html
- Building llvm with using clang: http://btorpey.github.io/blog/2015/01/02/building-clang/
- Using cmake to build llvm:  http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
- Run an LLVM Pass Automatically with Clang: http://adriansampson.net/blog/clangpass.html
- LLVM for Grad Students: http://adriansampson.net/blog/llvm.html

