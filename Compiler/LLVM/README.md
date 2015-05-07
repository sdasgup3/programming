```
If one wants to see the LLVM code produced by clang for a C program, they can do:
$ clang -S -emit-llvm myfile.c -o myfile.ll

If one wants to create an LLVM bytecode file from a C file, they can use:
$ clang -c -emit-llvm myfile.c -o myfile.bc

The bytecode files are not human readable. However you can transform from bytecode to text and 
vice versa using some LLVM tools:

$ llvm-dis myfile.bc -o myfile.ll
$ llvm-as myfile.ll -o myfile.bc

For more about various LLVM tools: http://llvm.org/docs/CommandGuide/index.html
```
