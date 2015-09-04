/* ************************************************************************* 
* NAME: colorconv/colorconv.h
*
* DESCRIPTION:
*
* this library uses the SSEn/MMX processor features to convert yuv ->rgba.
* on powerpc it uses altivec
* on it ARM Cortex-A it uses neon 
*
* yuv422rgb_scalar_fixed converts using only integer operations
* yuv422rgb_scalar_float converts using floating point math and no vector
*         operations (other than what the compiler generates by itself).
*         so yuv422rgb_scalar_float is the baseline.
*
* yuv422rgb_mmx1 uses integer operations vectorized with the MMX features
* yuv422rgb_sse1 converts using floating point operations vectorized by
*         using the SSE1 instructions.
*
* yuv422rgb_mmx2 uses integer operations vectorized with the MMX features
*         that were augmented by SSE2.
* yuv422rgb_sse2 converts using floating point operations vectorized by
*         using the SSE2 instructions.
*
* yuv422rgb_neon_int uses integer operations vectorized with the neon
*         features.
* yuv422rgb_neon_float converts using floating point operations vectorized
*         by neon features
*
* yuv422rgb_avc_int uses integer operations vectorized with the altivec
*         features
* yuv422rgb_avc_float  converts using floating point operations vectorized
*         by altivec features
*
* all functions have the same arguments and return type. the idea is to
* make it easy to use whichever one your processor has the features
* to support.
*
* SSEn/MMX are vector processing instructions that fall into the category
* of Single Instruction Multiple Data (SIMD) processing.
*
* this code uses the intel, altivec, and neon intrinsics that are
* implemented in gcc.
*
* SIMD has the larger effect when you're doing a lot to a little bit of data
* than if you're doing a little to a lot of data.
* loading data into and out of SIMD takes a fair amount of time
*
*
* PROCESS:
*
* #include "colorconv.h"
* 
* Cpusimdfeatures_t supported;
* int width, height; --  of image in pixels
* int byte_count; -- number of bytes in YUV image (2 * width * height) 
* unsigned char * sourcep = malloc(byte_count); -- source yuv image
* unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 
*
* some_int =  test_cpu_simd_features(&supported);
*
* if (0 == some_int)
*    -- supported structure now contains data on which features are supported
* else
*    -- there was an error trying to find out processor features
*
*
* -- build your YUYV image in sourcep
* -- use the contents of the supported structure to see what your
* -- CPU will support.
* -- use yuv422rgb_* to convert from YUYV to RGBA
*
*
* use print_cpusimdfeatures_t to dump out the contents of the supported
* structure.
*
*
* GLOBALS: none
*
* REFERENCES:
*
* http://gcc.gnu.org/ml/gcc-patches/2008-03/msg00803.html
* http://softpixel.com/~cwright/programming/colorspace/yuv/
* http://en.wikipedia.org/wiki/YUV#Converting_between_Y.27UV_and_RGB
*
* LIMITATIONS: researched only on x86, x86_64, altivec, arm v7 neon
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   22-Dec-09          initial coding                        gpk
*   21-Feb-10  added neon functions, neon slot in            gpk
*              Cpusimdfeatures_t
*    9-Oct-10  added altivec slot in Cpusimdfeatures_t       gpk
*   21-jan-11  added altivec entries to the file header      gpk
*              block comments; added public domain blurb
*   26-Mar-11  added yuv422rgb_float, yuv422rgb_scalar_fixed gpk
*              yuv422rgb_float_vector, yuv422rgb_fixed_vector 
*
* TARGET: GNU C version 4 or higher
*
* The contents of this file are in the public domain. If it breaks, you 
* get to keep both pieces.
*
* ************************************************************************* */

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

/* CPU feature test code to determine what features your processor  */
/* supports (and which of these functions you can use).  */
/* NOTE: if you change this structure, change  test_cpu_simd_features, */
/* print_cpusimdfeatures_t to match.   */
/* 3/1/11 gpk added xop, fma4   */
  
typedef struct cpusimdfeatures_t {
  int mmx;
  int sse1;
  int sse2;
  int sse3;
  int sse4_1;
  int sse4_2;
  int sse4_a;
  int sse5;
  int xop;
  int fma4;
  int three_d_now;
  int three_d_now2;
  int neon;
  int altivec;
} Cpusimdfeatures_t;

/* test_cpu_simd_features - modify supported to show processor features,  */
/* return 0 on success  */
int test_cpu_simd_features(Cpusimdfeatures_t * supported);

/* print_cpusimdfeatures_t - print prompt, followed by the contents  */
/* of supported in human-readable form  */
void print_cpusimdfeatures_t(char * prompt, Cpusimdfeatures_t supported);


/* scalar (non-vector) functions from scalar.c  */
void yuv422rgb_scalar_float(const unsigned char * sourcep, int source_byte_count,
		    unsigned char * destp);
void yuv422rgb_scalar_fixed(const unsigned char * sourcep, int source_byte_count,
		 unsigned char * destp);

/* SSE1 functions defined in sse1.c  */
void  yuv422rgb_mmx1(const unsigned char * sourcep, int source_byte_count,
		   unsigned char * destp);
void yuv422rgb_sse1(const unsigned char * sourcep, int source_byte_count,
		  unsigned char * destp);

/* SSE2 functions defined in sse2.c  */
void  yuv422rgb_mmx2(const unsigned char * sourcep, int source_byte_count,
		   unsigned char * destp);
void yuv422rgb_sse2(const unsigned char * sourcep, int source_byte_count,
		  unsigned char * destp);

/* ARM neon functions in neon.c  */
  
void  yuv422rgb_neon_int(const unsigned char * sourcep, int source_byte_count,
			 unsigned char * destp);
void yuv422rgb_neon_float(const unsigned char * sourcep, int source_byte_count,
			  unsigned char * destp);

  /* PowerPC altivec functions in altivec.c */

void yuv422rgb_avc_int(const unsigned char * sourcep, int source_byte_count,
			 unsigned char * destp);
void yuv422rgb_avc_float(const unsigned char * sourcep, int source_byte_count,
			 unsigned char * destp);

/* general functions that call whatever vector support we have. if we  */
/* don't have any, return 0  */
  
int yuv422rgb_fixed_vector (const unsigned char * sourcep,
			    int source_byte_count,  unsigned char * destp);
int yuv422rgb_float_vector(const unsigned char * sourcep,
			    int source_byte_count, unsigned char * destp);
  
/* the general function that check processor features at run time  */

void  yuv422rgb_fixed(const unsigned char * sourcep,
			     int source_byte_count, unsigned char * destp);
void  yuv422rgb_float(const unsigned char * sourcep,
			     int source_byte_count, unsigned char * destp);
  
#ifdef __cplusplus
}
#endif /* cplusplus */

