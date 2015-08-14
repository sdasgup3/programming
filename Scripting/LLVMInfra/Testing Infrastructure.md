Multisource build process
=========================
//generate bc
lang -S arg.c -o Output/arg.ll -emit-llvm
lvm-as Output/arg.ll -o Output/arg.bc

//linking
lvm-link Output/arg.bc Output/gram.bc -o Output/make_dparser.linked.rbc

//Optimize with compile time opts
RunSafely.sh 
--show-errors -t "/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/tools/timeit" 500
/dev/null // Input to Runsafely
Output/make_dparser.linked.bc.opt  // Output
otp //commands 
-std-compile-opts -info-output-file=/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/MultiSource/Applications/d/Output/make_dparser.linked.bc.info -stats -time-passes  Output/make_dparser.linked.rbc -o Output/make_dparser.linked.bc

//Optimize with compile time opts
/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/RunSafely.sh --show-errors -t "/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/tools/timeit" 500 /dev/null Output/make_dparser.llvm.bc.opt \
          /home/sdasgup3/Documents/llvm/llvm.obj/Release+Asserts/bin/opt -std-link-opts  \
                -info-output-file=/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/MultiSource/Applications/d/Output/make_dparser.llvm.bc.info \
                        -stats -time-passes Output/make_dparser.linked.bc  -o Output/make_dparser.llvm.bc

//Generate native executable using gcc
g++ -o Output/make_dparser.native Output/arg.o Output/gram.o 

//Run it
/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/RunSafely.sh -t "/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/tools/timeit-target" 500 /dev/null Output/make_dparser.out-nat Output/make_dparser.native -v /home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/MultiSource/Applications/d/grammar.g

//Use llc to produce assemble .s 
/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/RunSafely.sh --show-errors -t "/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/tools/timeit" 500 /dev/null Output/make_dparser.llc.s.llc \
          /home/sdasgup3/Documents/llvm/llvm.obj/Release+Asserts/bin/llc -asm-verbose=false -O3  Output/make_dparser.llvm.bc -o Output/make_dparser.llc.s -info-output-file=/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/MultiSource/Applications/d/Output/make_dparser.llc.s.info -stats -time-passes
//Use gcc to generate executable
gcc Output/make_dparser.llc.s -o Output/make_dparser.llc  -lm   -m64 -fomit-frame-pointer
//Execute it
/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/RunSafely.sh -t "/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/tools/timeit-target" 500 /dev/null Output/make_dparser.out-llc Output/make_dparser.llc -v /home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/MultiSource/Applications/d/grammar.g

//Run lli
/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/RunSafely.sh -t "/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/tools/timeit-target" 500 /dev/null Output/make_dparser.out-jit /home/sdasgup3/Documents/llvm/llvm.obj/Release+Asserts/bin/lli -info-output-file=/home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.obj/MultiSource/Applications/d/Output/make_dparser.out-jit.info -stats -time-passes -force-interpreter=false --disable-core-files  Output/make_dparser.llvm.bc -v /home/sdasgup3/Documents/llvmpa/llvmpa.tests/klee-test-suite.src/MultiSource/Applications/d/grammar.g

