### High level assembly syntax
-  AT&T syntax (used by as,gas) & Intel (use by nasm,tasm, masm)
- [Good read](http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm)

#### Commands 
- gcc -S -masm=att/intel test.c ; clang/gcc test.s
- clang -S -mllvm --x86-asm-syntax=att/intel test.c ; clang/gcc test.s
- clang -S -masm=att/intel test.c;  clang/gcc test.s
- [Watch out!] While compiling using clang in intel format,  the ASM file does not have a ".intel_syntax noprefix" directive at the top, and thus can't be parsed by the gcc/clang assembler (because that defaults to AT&T mode).
 

### x86/x86_64 Syntax
- push arg
  - SP -- ; MEM[SP] = arg 
- pop arg
  - arg = MEM[SP]; SP++ 
