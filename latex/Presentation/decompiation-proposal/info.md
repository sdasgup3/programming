###Motivation for executable analysis Traditional
Traditional source-code analysis
frameworks have limited applicability in several scenarios which can be 
eliminated by  executable-level tools.
Some scenarios are:
1. Absence of source-code 
There are several circumstances where the original
source-code is not accessible. Some of the most prevalent reasons are listed
below: 
IP-protected software → Third-party library and software components



Challenges ============ During the assembly process some information si lost:
1. program data is stored in the same raw binary format as the machine code
instructions.  This means that it can be difficult to determine which parts of
the program are actually instructions. Notice that you can disassemble raw
data, but the resultant assembly code will be nonsensical. 

2. Textual information from the assembly source code file, such as variable
names, label names, and code comments are all destroyed during assembly. When
you disassemble the code, the instructions will be the same, but all the other
helpful information will be lost. The code will be accurate, but more difficult
to read.



Two well know taregts: amd64/x86_64/EMT64 32 bit (x86/IA-32)




Commands ============ gcc -masm=intel/att -S x.c




x_86 instruction ================= push arg

This instruction decrements the stack pointer and stores the data specified as
the argument into the location pointed to by the stack pointer.

pop arg

This instruction loads the data store




McSema translates x86 machine code into LLVM bitcode.

Why would we do such a crazy thing?

Because we wanted to analyze existing binary applications, and reasoning about
LLVM bitcode is much easier than reasoning about x86 instructions. Not only is
it easier to reason about LLVM bitcode, but it is easier to manipulate and
re-target bitcode to a different architecture. There are many program analysis
tools (e.g. KLEE, PAGAI, LLBMC) written to work on LLVM bitcode that can now be
used on existing applications. Additionally it becomes much simpler to
transform applications in complex ways while maintaining original application
functionality.

There are other x86 to LLVM bitcode translators, but McSema has several
advantages:

    McSema separates control flow recovery from translation, permitting the use
    of custom control flow recovery front-ends.  McSema supports FPU
    instructions.  McSema is open source and licensed under a permissive
    license.  McSema is documented, works, and will be available soon after our
    REcon talk.


dagger; libbeauty, fracture    















To Know ========

080482cc <__gmon_start__@plt-0x10>: 80482cc:	ff 35 ac 96 04 08    	pushl
0x80496ac 80482d2:	ff 25 b0 96 04 08    	jmp    *0x80496b0 80482d8: 00
00                	add    %al,(%eax) ...

080482dc <__gmon_start__@plt>: 80482dc:	ff 25 b4 96 04 08    	jmp *0x80496b4
80482e2:	68 00 00 00 00       	push   $0x0 80482e7:	e9 e0 ff ff ff
jmp    80482cc <_init+0x30>

080482ec <__libc_start_main@plt>: 80482ec:	ff 25 b8 96 04 08    	jmp
*0x80496b8 80482f2:	68 08 00 00 00       	push   $0x8 80482f7:	e9 d0
ff ff ff       	jmp    80482cc <_init+0x30>

080482fc <printf@plt>: 80482fc:	ff 25 bc 96 04 08    	jmp    *0x80496bc
8048302:	68 10 00 00 00       	push   $0x10 8048307:	e9 c0 ff ff ff
jmp    80482cc <_init+0x30>

0804830c <atoi@plt>: 804830c:	ff 25 c0 96 04 08    	jmp    *0x80496c0
8048312:	68 18 00 00 00       	push   $0x18 8048317:	e9 b0 ff ff ff
jmp    80482cc <_init+0x30> ???  '

Single Source ==============
dsand@dsand-Inspiron-3521:~/Github/llvm-test-suite-mcsema/SingleSource$ find .
-name "*.simple" | wc 627     627   39545
dsand@dsand-Inspiron-3521:~/Github/llvm-test-suite-mcsema/SingleSource$ find .
-name "*.simple.lifted" | wc 190     190   16520

Dir                     Total     Pass    FAil

Regression              49        39      10 UnitTest                94 72 22
Benchmark               45        19      26

long time ././Shootout/Output/fib2.simple.mcsema/fib2.simple.lifted
Benchmarks/McGill/Output/exptree.simple.mcsema/exptree.simple.lifted

erro 2.14 GLIBC?? on other machines are those runnable with bindescend on my
machine why std_args does not run why somes of them re not coveting why some of
them are chrashing

