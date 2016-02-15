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
 ## Copy llvm-src/projects/sample
```
$ mkdir build
$ cd build
$ ../sample/configure --with-llvmsrc=/home/sdasgup3/Documents/llvm/llvm.src/ --with-llvmobj=/home/sdasgup3/Documents/llvm/llvm.obj/
$ make ENABLE_OPTIMIZED=1   
```

## Links ##
- For more about various LLVM tools: http://llvm.org/docs/CommandGuide/index.html
- For moe information about creating a LLVM project :  http://llvm.org/releases/3.7.1/docs/Projects.html

