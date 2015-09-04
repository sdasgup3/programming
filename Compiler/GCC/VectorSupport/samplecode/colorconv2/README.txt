colorconv2/README.txt

This is the colorconv library. It converts from YUV422 color space
(produced by some cameras) to RGBA. There are a couple of options
here depending on what your processor supports.

 If your processor supports SSE2 and MMX, there are functions that
convert images using those vector extensions. (This can be faster than SSE1.)

 If your processor supports SSE(1) and MMX, there are functions that
convert images using those vector extensions. (This can be faster than scalar
processing).

 If your processor supports NEON extensions, there are functions for
that.

 If your processor supports altivec extensions, there are functions for
that.

 If your processor doesn't support vector extensions, there are scalar
(non-vector) versions of those functions.

 The vectorized functions (SSE/MMX/SSE2/neon/altivec) assume that the
image size is a multiple of 8 and that it's aligned on a 16-byte boundary.

 The *_sse[12] functions use a different set of coefficients than the
scalar functions do. So expect a small difference in results. (If they
need to be closer together, change the constants. The important thing
is to use the constants that your source uses to encode the video.)


 In testlib.c there is test code that shows how to use the library and
runs a timing test so you can see what runs fastest on your computer.

 In interface.c is the general function that checks CPU features and
calls the right function for the features your CPU supports.


FILES:
-----

altivec.c - Freescale Altivec specific code
colorconv.h - header file for the library
cputest.c - code to test what CPU you have
cvt2ps.h - a macro to turn vectors of floats to vectors of chars
extract.h - bug fix for _mm_cvtpu16_ps, _mm_cvtpi16_ps
i386 - directory of architecture specific Makefile, binaries for X86
interface.c - external interface to the library
Makefile - bounce you to architecture-specific directory
neon.c - code using neon intrinsics for ARM
ppc64  - directory of architecture specific Makefile, binaries 64-bit altivec
printValues.c - generic print functions. Made into a library when "make"
                 is executed
printValues.h - prototypes for printValues.c
rgbcalc.c - test program to translate YUYV to RGBA
README.txt - this file
scalar.c - fallback scalar code
sse1.c - x86 code for SSE1/MMX capable processors
sse2.c - x86 code for SSE2/MMX capable processors
testlib.c - timing test code for the library
unknown  - directory of architecture specific Makefile, binaries for neon
x86_64 - directory for 64-bit X86



